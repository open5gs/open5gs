import React from 'react';

export default Page => class DefaultPage extends React.Component {
  static async getInitialProps (ctx) {
    let username = null;
    if (ctx.req && ctx.req.user) {
        username = ctx.req.user.username;
    } 

    const pageProps = Page.getInitialProps && Page.getInitialProps(ctx);
    return { 
      ...pageProps,
      Username: username,
      isAuthenticated: !!username
     };
  }

  render() {
    return (<Page { ...this.props } />);
  }
}