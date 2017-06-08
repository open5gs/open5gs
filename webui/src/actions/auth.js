import { createAction } from 'redux-actions';

export const AUTH = {
  LOGIN_REQUEST: 'auth/LOGIN_REQUEST',
  LOGIN_SUCCESS: 'auth/LOGIN_SUCCESS',
  LOGIN_FAILURE: 'auth/LOGIN_FAILURE',
  LOGOUT: 'auth/LOGOUT'
}

export const loginRequest = createAction(AUTH.LOGIN_REQUEST); // { username, password }