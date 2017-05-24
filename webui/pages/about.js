import Link from 'next/prefetch'
import React from 'react'
import withSession from '../lib/with-session'
import LogoutButton from '../components/logout-button'

const About = ({ session, isLoggedIn }) => {
  return (
    <div>
      <h1>Welcome to the Next.js auth example</h1>
      {!isLoggedIn && <p><Link href='/login'><a>Login</a></Link></p>}
      {isLoggedIn && (
        <div>
          <p>Welcome back {session.user.username}</p>
          <LogoutButton session={session}>Log out</LogoutButton>
        </div>
      )}
      <p><Link href='/secret'><a>Restricted page</a></Link></p>
    </div>
  )
}

// withSession can only be used on top level components (routes inside the pages directory)
export default withSession(About)