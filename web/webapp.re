Pkg_index_data.load()
|> Repromise.Rejectable.map( index => {
  ReactDOMRe.renderToElementWithId(<Components.Pkg_list index />, "app") |> ignore;
  Repromise.Rejectable.resolved();
  });

