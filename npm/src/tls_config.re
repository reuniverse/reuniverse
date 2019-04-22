open Lwt.Infix;

let from_ca: _ => Httpkit_lwt.Client.Https.tls_config =
  ca => {
    {
      tracer:
        Some(
          sexp =>
            Printf.eprintf(
              "Trace: %s\n%!",
              Sexplib.Sexp.to_string_hum(sexp),
            ),
        ),
      tls_client: (req, socket) => {
        let host = Httpkit.Client.Request.uri(req) |> Uri.host_with_default;
        X509_lwt.authenticator(ca)
        >>= (
          authenticator => {
            let client =
              Tls.Config.client(
                ~peer_name="api.npms.io",
                ~authenticator,
                ~alpn_protocols=["http/1.1"],
                (),
              );
            Tls_lwt.Unix.client_of_fd(
              ~trace=
                sexp =>
                  Printf.eprintf(
                    "Trace: %s\n%!",
                    Sexplib.Sexp.to_string_hum(sexp),
                  ),
              client,
              ~host,
              socket,
            );
          }
        );
      },
    };
  };
