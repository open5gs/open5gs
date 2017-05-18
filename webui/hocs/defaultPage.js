import React from 'react';
import Layout from '../components/layout';

export default Page => class DefaultPage extends React.Component {
  static async getInitialProps (ctx) {
    let username = null;
    let role = null;
    if (process.browser) {
      username = 'admin';
      role = 'admin';
    } else {
      if (ctx.req && ctx.req.user) {
          username = ctx.req.user.username;
          if (ctx.req.user.UserRole) {
            role = ctx.req.user.UserRole.role;
          }
      } 
    }

    const pageProps = Page.getInitialProps && Page.getInitialProps(ctx);
    return { 
      ...pageProps,
      Username: username,
      Role: role,
      currentUrl: ctx.pathname,
      isAuthenticated: !!username
     };
  }

  render() {
    return (
      <div>
        <Layout { ...this.props }>
          <Page { ...this.props } />
        </Layout>
      </div>
    );
  }
}