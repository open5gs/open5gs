import React from 'react';
import Link from 'next/link';
import defaultPage from '../hocs/defaultPage';

const Index = ({ loggedUser, isAuthenticated }) => {
  return (
    <div>
      <h1>Hello Worlds!</h1>
      { loggedUser }
      {isAuthenticated && <p>Authenticated!!</p>}
      {!isAuthenticated && <p>Not Authriozed</p>}
      <p><Link href='/login'><a>Login</a></Link></p>
      <p><Link href='/logout'><a>Logout</a></Link></p>
    </div>
  );
}

export default defaultPage(Index);