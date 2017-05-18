import React from 'react';
import PropTypes from 'prop-types';
import Link from 'next/link';
import DefaultPage from '../hocs/defaultPage';

const Index = ({ isAuthenticated }) => {
  return (
    <div>
      <h1>Hello Worlds!</h1>
      {isAuthenticated && <p>Authenticated!!</p>}
      {!isAuthenticated && <p>Not Authriozed</p>}
      <p><Link prefetch href='/login'><a>Login</a></Link></p>
      <p><Link prefetch href='/logout'><a>Logout</a></Link></p>
      <p><Link prefetch href='/secret'><a>Secure Page</a></Link></p>
    </div>
  );
}

Index.propTypes = {
  isAuthenticated: PropTypes.bool.isRequired
}

export default DefaultPage(Index);