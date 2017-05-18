import Link from 'next/link'
import Head from 'next/head'

const Layout = ({ 
  children, title = 'This is the default title', 
  isAuthenticated, currentUrl }) => {
  return (
    <div>
      <Head>
        <title>{ title }</title>
        <meta charSet='utf-8' />
        <meta name='viewport' content='initial-scale=1.0, width=device-width' />
      </Head>
      <header>
        <nav>
          <Link prefetch key='/about' href='/about'><a>About</a></Link> |
          <Link prefetch key='/contact' href='/contact'><a>Contact</a></Link>
          {isAuthenticated && <p>isAuthenticated</p>}
          {currentUrl && <p>{currentUrl}</p>}
        </nav> 
      </header>

      { children }

      <footer>
        I'm here to say 
      </footer>
    </div>
  );
}

export default Layout;