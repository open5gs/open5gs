import React from 'react';

export default Page => class DefaultPage extends React.Component {
  static getInitialProps (ctx) {
    const loggedUser = 'admin'
    const pageProps = Page.getInitialProps && Page.getInitialProps(ctx);
    return { 
      ...pageProps,
      loggedUser,
      isAuthenticated: !!loggedUser
     };
  }

  render() {
    return (<Page { ...this.props } />);
  }
}
