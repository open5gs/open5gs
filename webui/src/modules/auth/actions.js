export const AUTH = {
  LOGIN_REQUEST: 'auth/LOGIN_REQUEST',
  LOGIN_SUCCESS: 'auth/LOGIN_SUCCESS',
  LOGIN_FAILURE: 'auth/LOGIN_FAILURE',
  LOGOUT: 'auth/LOGOUT'
}

function action(type, payload = {}) {
  return {type, ...payload}
}

export const loginRequest = (username, password) => action(AUTH.LOGIN_REQUEST, {username, password})
export const loginSuccess =  (username, role) => action(AUTH.LOGIN_SUCCESS, {username, role})
export const loginFailure = () => action(AUTH.LOGIN_FAILURE)
export const logout = () => action(AUTH.LOGOUT)