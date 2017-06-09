import { take, fork, cancel, call, put } from 'redux-saga/effects';

import Session from 'services/session';
import { AUTH, loginSucess} from 'actions/auth';
/* import { UI, toggleSidebar } from 'actions/ui'; */

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
    const { payload } = yield take(AUTH.LOGIN_REQUEST);
    const { username, password } = payload;
    const task = yield fork(authroize, username, password);
    const action = yield take([AUTH.LOGOUT, AUTH.LOGIN_FAILURE]);
    if (action.type === AUTH.LOGOUT)
      yield cancel(task);
  }
}

export default function* () {
  yield loginFlow();
}

/*
function* testCode() {
  yield put(toggleSidebar());
}

function* loginFlow() {
  yield takeEvery(UI.SELECT_VIEW, testCode)
}

export default function* () {
  yield loginFlow();
}
*/