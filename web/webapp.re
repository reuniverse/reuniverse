module Main = {
  [@react.component]
  let make = () => {
    let (filter, setFilter) = React.useState(() => "");
    Design_system.(
      <section style=Design_system.global_style>
        <Background />
        <Nav>
          <Logo />
          <Search_input onChange=setFilter placeholder="Search..." />
        </Nav>
        <Search.App filter />
      </section>
    );
  };
};

ReactDOMRe.renderToElementWithId(<Main />, "app");
