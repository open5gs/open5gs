import PropTypes from 'prop-types';

import withSession from '../lib/with-session';
import Login from '../components/Login';
import Layout from '../components/Layout';

const Restricted = (Component) => {
  const checkAuth = (props) => {
    return props.isLoggedIn ? <Component {...props} /> : <Login/>
  }

  return withSession(checkAuth);
}

const Index = Restricted(({session}) => (
  <div>
    <Layout session={session} />
  </div>
  )
)

Index.propTypes = {
  session: PropTypes.object.isRequired
};

export default Index;