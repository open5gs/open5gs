import { combineReducers } from 'redux';

import auth from './auth';
import crud from './crud';
import ui from './ui';

export default combineReducers({
  auth,
  crud,
  ui
});