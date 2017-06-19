import { all, fork } from 'redux-saga/effects';
import auth from './auth';
import crud from './crud';

export default function* rootSaga() {
  yield all([
    fork(auth),
    fork(crud)
  ])
}