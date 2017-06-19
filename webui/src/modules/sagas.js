import { all, fork } from 'redux-saga/effects';
import auth from './auth/sagas';
import crud from './crud/sagas';

export default function* rootSaga() {
  yield all([
    fork(auth),
    fork(crud)
  ])
}
