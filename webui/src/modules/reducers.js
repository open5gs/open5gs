import { combineReducers } from 'redux';

import auth from './auth/reducers';
import crud from './crud/reducers';
import ui from './ui';

export default combineReducers({
  auth,
  crud,
  ui
});
