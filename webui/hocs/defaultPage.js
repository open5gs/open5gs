import React from 'react';

export default Page => class DefaultPage extends React.Component {
  static async getInitialProps (ctx) {
    let loggedUser = null;
    if (ctx.req && ctx.req.user) {
        loggedUser = ctx.req.user.username;
    } 

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