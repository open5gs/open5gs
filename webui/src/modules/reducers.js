import { combineReducers } from 'redux';

import auth from './auth/reducers';
import crud from './crud/reducers';
import sidebar from './sidebar';
import notifications from './notification/reducers';

export default combineReducers({
  auth,
  crud,
  sidebar,
  notifications
});
