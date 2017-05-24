import Link from 'next/link';

export default () => (
  <div>
    <h1>Unauthorized</h1>
    <p>You are not authorized to view this page.</p>
    <p><Link prefetch href='/login'><a>Please log in</a></Link></p>
    <p><Link prefetch href='/'><a>Back to homepage</a></Link></p>
  </div>
)