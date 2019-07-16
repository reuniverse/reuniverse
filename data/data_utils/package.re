let latest_major_versions = pkg => {
  let majors = Hashtbl.create(8);
  Model.Package.versions(pkg)
  |> List.iter(v =>
       switch (Model.Version.version(v) |> Semver.of_string) {
       | None => ()
       | Some(semver) =>
         let major = Semver.(semver.major);
         switch (Hashtbl.find_opt(majors, major)) {
         | None => Hashtbl.replace(majors, major, v)
         | Some(v') =>
           switch (Model.Version.version(v') |> Semver.of_string) {
           | None => Hashtbl.replace(majors, major, v)
           | Some(semver') =>
             switch (Semver.greater_than(semver, semver')) {
             | true => Hashtbl.replace(majors, major, v)
             | _ => ()
             }
           }
         };
       }
     );
  majors |> Hashtbl.to_seq_values;
};
