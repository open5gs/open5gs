import { combineReducers } from 'redux';

import auth from './auth';
import { crudReducer } from 'redux-crud-store'
import ui from './ui';

export default combineReducers({
  auth,
  db: crudReducer,
  ui
});