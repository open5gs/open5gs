import { take, fork, cancel, call, put } from 'redux-saga/effects';

import { AUTH, loginSuccess } from './actions';
import Session from 'modules/auth/session';

function* authorize(username, password) {
  try {
    const session = new Session();
    const sessionData = yield call(session.signin, username, password);
    const { role } = sessionData.user;
    yield put(loginSuccess, username, role);
  } catch (error) {
    yield put(loginFailure);
  } finally {
    if (yield cancelled()) {
      // .. put special cancellation handling code here
    }
  }
}

function* loginFlow() {
  while(true) {
    const { username, password } = yield take(AUTH.LOGIN_REQUEST);
    const task = yield fork(authorize, username, password);
    const action = yield take([AUTH.LOGOUT, AUTH.LOGIN_FAILURE]);
    if (action.type === AUTH.LOGOUT)
      yield cancel(task);
  }
}

export default function* () {
  yield loginFlow();
}