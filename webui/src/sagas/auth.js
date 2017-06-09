import { takeEvery, put, call, take } from 'redux-saga/effects';

import { AUTH } from 'actions/auth';
import { UI, toggleSidebar } from 'actions/ui';

function* testCode() {
  yield put(toggleSidebar());
}

function* loginFlow() {
  yield takeEvery(UI.SELECT_VIEW, testCode)
}

export default function* () {
  yield loginFlow();
}