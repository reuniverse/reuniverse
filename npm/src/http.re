module Request = {
  let create = (~headers=H2.Headers.empty, method, uri) => {
    H2.Request.create(
      ~scheme="https",
      ~headers,
      method,
      uri |> Uri.to_string,
    );
  };
};

let response_handler = (notify_response_received, response, response_body) =>
  switch (H2.Response.(response.status)) {
  | `OK =>
    let final_response = ref("");
    let rec read_response = () =>
      H2.Body.schedule_read(
        response_body,
        ~on_eof=
          () => Lwt.wakeup_later(notify_response_received, final_response^),
        ~on_read=
          (response_fragment, ~off, ~len) => {
            let response_fragment_string = Bytes.create(len);
            Lwt_bytes.blit_to_bytes(
              response_fragment,
              off,
              response_fragment_string,
              0,
              len,
            );
            let fragment = Bytes.unsafe_to_string(response_fragment_string);
            Logs.debug(m => m("Received fragment: %s", fragment));
            final_response := final_response^ ++ fragment;
            read_response();
          },
      );

    Logs.debug(m => m("Scheduled reading of response..."));
    read_response();
  | _ =>
    Format.fprintf(
      Format.err_formatter,
      "%a\n%!",
      H2.Response.pp_hum,
      response,
    );
    exit(1);
  };

let error_handler = _ => assert(false);

let send:
  (~host: string, ~port: int, ~body: option(string)=?, H2.Request.t) =>
  Lwt.t(string) =
  (~host, ~port, ~body=None, req) => {
    Lwt.Infix.(
      Lwt_unix.getaddrinfo(
        host,
        string_of_int(port),
        [Unix.(AI_FAMILY(PF_INET))],
      )
      >>= (
        addresses => {
          let socket = Lwt_unix.socket(Unix.PF_INET, Unix.SOCK_STREAM, 0);
          Lwt_unix.connect(socket, List.hd(addresses).Unix.ai_addr)
          >>= (
            () => {
              Logs.debug(m => m("Opening socket to: %s:%d", host, port));
              let (response_received, notify_response_received) = Lwt.wait();
              let response_handler =
                response_handler(notify_response_received);
              H2_lwt_unix.Client.create_connection(~error_handler, socket)
              >>= (
                conn => {
                  Logs.debug(m => m("Connected to: %s:%d", host, port));
                  let request_body =
                    H2_lwt_unix.Client.request(
                      conn,
                      req,
                      ~error_handler,
                      ~response_handler,
                    );
                  switch (body) {
                  | Some(text_to_send) =>
                    H2.Body.write_string(request_body, text_to_send)
                  | None => ()
                  };
                  H2.Body.close_writer(request_body);
                  Logs.debug(m => m("Sent request."));
                  response_received;
                }
              );
            }
          );
        }
      )
    );
  };

let get = (~host, ~port, req) => send(~host, ~port, req, ~body=None);
let post = send;
