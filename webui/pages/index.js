import React from 'react';
import Link from 'next/link';

const Index = () => {
  return (
    <div>
      <h1>Hello Worlds!</h1>
      <p><Link href='/login'><a>Login</a></Link></p>
      <p><Link href='/logout'><a>Logout</a></Link></p>
    </div>
  );
}

export default (Index);