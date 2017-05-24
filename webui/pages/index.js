import Package from '../package';
import withSession from '../lib/with-session';

import App from '../components/App';
import Header from '../components/Header';
import Login from '../components/Login';
import LogoutButton from '../components/logout-button';

const Restricted = (Component) => {
  const checkAuth = (props) => {
    return props.isLoggedIn ? <Component {...props} /> : <Login />
  }

  return withSession(checkAuth);
}

export default Restricted(({ session }) => {
  const title = 'NextEPC ' + Package.version;

  return (
    <App>
      <Header title={ title }>
      </Header>
        <div>
        <p>Welcome back {session.user.username}</p>
        <LogoutButton session={session}>Log out</LogoutButton>
        </div>
    </App>
  )
})


/**
 * The index page uses a layout page that pulls in header and footer components
import Link from 'next/link'
import React from 'react'
import Page from '../components/page'
import Layout from '../components/layout'

export default class extends Page {

  render() {
    return (
      <Layout session={this.props.session}>
        <h2>Under construction</h2>
        <ul>
          <li><Link prefetch href="/login"><a>Login</a></Link> - prefetch</li>
          <li><Link prefetch href="/about"><a>About</a></Link> - About</li>
        </ul>
      </Layout>
    )
  }
}
 */
