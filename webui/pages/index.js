import React from 'react';
import Link from 'next/prefetch';
import withSession from '../components/with-session';

const Index = ({ session, isLoggedIn }) => {
  return (
    <div>
      <h1>Hello Worlds!</h1>
    </div>
  );
}

export default withSession(Index);