/**
 * The index page uses a layout page that pulls in header and footer components
 */
import Link from 'next/link'
import React from 'react'
import Start from '../components/start'
import Layout from '../components/layout'
import Session from '../components/session'

export default class extends React.Component {

  static async getInitialProps({req}) {
    const session = new Session({req})
    return {session: await session.getSession(true)}
  }

  render() {
    return (
      <Layout session={this.props.session}>
        <h2>Under construction</h2>
        <ul>
          <li><Link prefetch href="/helloworld"><a>HelloWorld</a></Link> - The simplest possible example</li>
          <li><Link prefetch href="/login"><a>Login</a></Link> - prefetch</li>
          <li><Link prefetch href="/about"><a>About</a></Link> - About</li>
          <li><Link prefetch href="/secret"><a>Secret</a></Link> - Secret</li>
        </ul>
      </Layout>
    )
  }
}
