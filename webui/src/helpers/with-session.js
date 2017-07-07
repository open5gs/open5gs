import React from 'react';
import Session from 'modules/auth/session';

export default (Component) => class extends React.Component {
  static async getInitialProps (ctx) {
    const session = new Session({req: ctx.req});

    let initialProps = {};
    if (Component.getInitialProps) {
      initialProps = Component.getInitialProps({...ctx, session});
    }

    const sessionData = await session.getSession();
    let isLoggedIn = false;
    if (sessionData.user && sessionData.user.username) {
      isLoggedIn = true;
    }

    return {session: sessionData, isLoggedIn, ...initialProps};
  }

  render () {
    return <Component {...this.props} />
  }
}