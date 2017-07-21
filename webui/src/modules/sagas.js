import { all, fork } from 'redux-saga/effects';
import crud from './crud/sagas';

export default function* rootSaga() {
  yield all([
    fork(crud)
  ])
}
