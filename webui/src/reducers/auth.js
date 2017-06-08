import { handleActions } from 'redux-actions';
import { AUTH } from '../actions/auth';

const initialState = {
  username: '',
  password: ''
}

export default handleActions({
  [AUTH.LOGIN_REQUEST]: (state, action) => ({ 
    username: action.payload.username,
    password: action.payload.password 
  })
}, initialState);