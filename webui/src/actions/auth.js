import { createAction } from 'redux-actions';

export const AUTH = {
  LOGIN_REQUEST: 'auth/LOGIN_REQUEST',
  LOGIN_SUCCESS: 'auth/LOGIN_SUCCESS',
  LOGIN_FAILURE: 'auth/LOGIN_FAILURE',
  LOGOUT: 'auth/LOGOUT'
}

/*
  AuthAction.loginRequest
    payload : {
      username,
      password
    } 

  AuthAction.loginSuccess
    payload : {
      username,
      role
    } 

  AuthAction.loginFailure
    payload : null

  AuthAction.logout
    payload : null
*/
export const loginRequest = createAction(AUTH.LOGIN_REQUEST);
export const loginSuccess = createAction(AUTH.LOGIN_SUCCESS);
export const loginFailure = createAction(AUTH.LOGIN_FAILURE);
export const logout = createAction(AUTH.LOGOUT);