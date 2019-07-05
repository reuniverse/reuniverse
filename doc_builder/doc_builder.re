let download_all_packages = index => {
  Model.Index.(index.packages)
    |> List.to_seq
    |> Seq.map(download_one)
    |> Lwt_stream.of_seq


};

let build_from_index : Model.Index.t => _ = index => {
  Lwt_result.Infix.(
    index
    |> download_all_packages
    |> group_by_target
    |> List.to_seq
    |> Seq.map(build_doc)
    |> Lwt_stream.of_seq
    |> Lwt_stream.iter_p(res =>
         res >|= handle_doc_built >>= (_ => Lwt.return())
       )
    |> Lwt_main.run
  );
};
