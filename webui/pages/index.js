import PropTypes from 'prop-types';

import Package from '../package';
import withSession from '../lib/with-session';

import App from '../components/App';
import Header from '../components/Header';
import Login from '../components/Login';
import LogoutButton from '../components/logout-button';

const Restricted = (Component) => {
  const checkAuth = (props) => {
    return props.isLoggedIn ? <Component {...props} /> : <Login/>
  }

  return withSession(checkAuth);
}

const Index = Restricted(({session}) => {
  const title = 'NextEPC ' + Package.version;

  return (
    <App>
      <Header title={title}>
      </Header>
        <div>
        <p>Welcome back {session.user.username}</p>
        <LogoutButton session={session}>Log out</LogoutButton>
        </div>
    </App>
  )
})

Index.propTypes = {
  session: PropTypes.object.isRequired
};

export default Index;