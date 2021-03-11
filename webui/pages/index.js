import PropTypes from 'prop-types';
import withRedux from 'next-redux-wrapper';

import { initStore } from 'modules/store.js';
import withSession from 'helpers/with-session';

import Auth from 'containers/Auth';
import App from 'containers/App';

const Restricted = (Component) => {
  const checkAuth = (props) => {
    return props.isLoggedIn ? <Component {...props} /> : <Auth/>
  }

  return withSession(checkAuth);
}

const Index = Restricted(({session}) => (
  <div>
    <App session={session} />
  </div>
  )
)

Index.propTypes = {
  session: PropTypes.object.isRequired
};

export default withRedux(initStore)(Index);
