import { combineReducers } from 'redux';

import crud from './crud/reducers';
import sidebar from './sidebar';
import notifications from './notification/reducers';

export default combineReducers({
  crud,
  sidebar,
  notifications
});
