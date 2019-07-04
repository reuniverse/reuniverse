module Main = {
  [@react.component]
  let make = () => {
    let (filter, setFilter) = React.useState(() => "");
    Design_system.(
      <Global>
        <Background />
        <Nav>
          <Logo />
          <Search_input onChange=setFilter placeholder="Search..." />
        </Nav>
        <Search.App filter />
        <Footer />
      </Global>
    );
  };
};

ReactDOMRe.renderToElementWithId(<Main />, "app");
