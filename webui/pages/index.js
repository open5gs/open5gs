import React from 'react';
import Link from 'next/link';
import withSession from '../components/with-session';

const Index = ({ session, isLoggedIn }) => {
  return (
    <div>
      <h1>Hello Worlds!</h1>
      {!isLoggedIn && <p><Link href='/login'><a>Login</a></Link></p>}
      {isLoggedIn && <p><Link href='/logout'><a>Logout</a></Link></p>}
    </div>
  );
}

export default withSession(Index);