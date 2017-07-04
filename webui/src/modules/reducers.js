import { combineReducers } from 'redux';

import auth from './auth/reducers';
import crud from './crud/reducers';
import ui from './ui';
import notifications from './notification/reducers';

export default combineReducers({
  auth,
  crud,
  ui,
  notifications
});
