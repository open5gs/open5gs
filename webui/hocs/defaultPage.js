import React from 'react';

export default Page => class DefaultPage extends React.Component {
  static async getInitialProps (ctx) {
    let username = null;
    let role = null;
    if (ctx.req && ctx.req.user) {
        username = ctx.req.user.username;
        if (ctx.req.user.UserRole) {
          role = ctx.req.user.UserRole.role;
        }
    } 

    const pageProps = Page.getInitialProps && Page.getInitialProps(ctx);
    return { 
      ...pageProps,
      Username: username,
      Role: role,
      isAuthenticated: !!username
     };
  }

  render() {
    return (<Page { ...this.props } />);
  }
}