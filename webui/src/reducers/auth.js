import { handleActions } from 'redux-actions';
import { AUTH } from 'actions/auth';

const initialState = {
  isAuthenticating: false,
  isAuthenticated: false,
  session : {
    username : '',
    role: ''
  }
}

export default handleActions({
  [AUTH.LOGIN_REQUEST]: (state, action) => ({ 
    ...state,
    isAuthenticating: true,
    isAuthenticated: false,
    session : {
      ...state.session,
      username: action.payload.username,
      role: ''
    }
  }),
  [AUTH.LOGIN_SUCCESS]: (state, action) => ({ 
    ...state,
    isAuthenticating: false,
    isAuthenticated: true,
    session : {
      ...state.session,
      username: action.payload.username,
      role: action.payload.role
    }
  }),
  [AUTH.LOGIN_FAILURE]: (state, action) => ({ 
    ...state,
    isAuthenticating: false,
    isAuthenticated: false,
    session : {
      ...state.session,
      username: '',
      role: ''
    }
  }),
  [AUTH.LOGOUT]: (state, action) => ({ 
    ...state,
    isAuthenticating: false,
    isAuthenticated: false,
    session : {
      ...state.session,
      username: '',
      role: ''
    }
  })
}, initialState);