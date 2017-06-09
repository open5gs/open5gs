import { handleActions } from 'redux-actions';
import { AUTH } from '../actions/auth';

const initialState = {
  isLoggedIn: false,
  session : {
    username : '',
    role: ''
  }
}

export default handleActions({
  [AUTH.LOGIN_SUCCESS]: (state, action) => ({ 
    ...state,
    isLoggedIn: true,
    session : {
      ...state.session,
      username: action.payload.username,
      role: action.payload.role
    }
  }),
}, initialState);