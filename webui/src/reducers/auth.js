import { AUTH } from 'actions/auth';

const initialState = {
  isAuthenticating: false,
  isAuthenticated: false,
  session : {
    username : '',
    role: ''
  }
}

function auth(state = initialState, action) {
  switch (action.type) {
    case AUTH.LOGIN_REQUEST:
      return {
        ...state,
        isAuthenticating: true,
        isAuthenticated: false,
        session : {
          ...state.session,
          username: action.username,
          role: ''
        }
      }
    case AUTH.LOGIN_SUCCESS:
      return {
        ...state,
        isAuthenticating: false,
        isAuthenticated: true,
        session : {
          ...state.session,
          username: action.username,
          role: action.role 
        }
      }
    case AUTH.LOGIN_FAILURE:
      return {
        ...state,
        isAuthenticating: false,
        isAuthenticated: false,
        session : null
      }
    case AUTH.LOGOUT:
      return {
        ...state,
        isAuthenticating: false,
        isAuthenticated: false,
        session : null
      }
    default:
      return state;
  }
}

export default auth;