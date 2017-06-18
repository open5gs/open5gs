import { all } from 'redux-saga/effects';
import auth from './auth';
import crud from './crud';

export default function* rootSaga() {
  yield all([
    auth(),
    crud()
  ])
}