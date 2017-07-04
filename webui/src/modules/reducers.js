import { combineReducers } from 'redux';

import auth from './auth/reducers';
import crud from './crud/reducers';
import ui from './ui';
import notification from './notification/reducer';

export default combineReducers({
  auth,
  crud,
  ui,
  notification
});
