import axios from 'axios';
import { all, takeEvery, put, call, take, fork } from 'redux-saga/effects';
import { CRUD } from './actions';
import Session from 'modules/auth/session';

const crudApi = (method, url, csrf, authToken, { params, data } = {} ) => {
  let headers = { 'X-CSRF-TOKEN': csrf }
  if (authToken) {
    headers['Authorization'] = "Bearer " + authToken
  }
  return axios({
    baseURL: '/api/db',
    headers: headers,
    method,
    url,
    params,
    data
  });
}

function* crudEntity(action) {
  const { method, url, params, data } = action.payload;
  const { success, failure } = action.meta;
  const meta = {
    ...action.meta,
    fetchedAt: Date.now()
  }

  try {
    const sessionData = new Session();
    const csrf = ((sessionData || {}).session || {}).csrfToken;
    const authToken = ((sessionData || {}).session || {}).authToken;
    const response = yield call(crudApi, method, url, csrf, authToken, { params, data })
    yield put({ meta, type: success, payload: response })
  } catch (error) {
    yield put({ meta, type: failure, payload: error, error: true })
  }
}

function* watchFetch() {
  while(true) {
    const action = yield take(CRUD.FETCH);
    yield fork(crudEntity, action);
  } 
}

function* watchFetchOne() {
  while(true) {
    const action = yield take(CRUD.FETCH_ONE);
    yield fork(crudEntity, action);
  } 
}

function* watchCreate() {
  while(true) {
    const action = yield take(CRUD.CREATE);
    yield fork(crudEntity, action);
  } 
}

function* watchUpdate() {
  while(true) {
    const action = yield take(CRUD.UPDATE);
    yield fork(crudEntity, action);
  } 
}

function* watchDelete() {
  while(true) {
    const action = yield take(CRUD.DELETE);
    yield fork(crudEntity, action);
  } 
}

export default function* () {
  yield all([
    fork(watchFetch),
    fork(watchFetchOne),
    fork(watchCreate),
    fork(watchUpdate),
    fork(watchDelete)
  ])
}
