import axios from 'axios';
import { all, takeEvery, put, call } from 'redux-saga/effects';
import { CRUD } from 'actions/crud';

const crudApi = (method, url, { params, data } = {} ) => {
  return axios({ baseURL: '/api/db', method, url, params, data })
    .then(response => response.data);
}

function* crudEntity(action) {
  const { method, url, params, data } = action.payload;
  const { success, failure } = action.meta;
  const meta = {
    ...action.meta,
  }

  try {
    const response = yield call(crudApi, method, url, { params, data })
    yield put({ meta, type: success, payload: response })
  } catch (error) {
    yield put({ meta, type: failure, payload: error, error: true })
  }
}

function* watchFetch() {
  yield takeEvery(CRUD.FETCH, crudEntity)
}

export default function* () {
  yield all([
    watchFetch()
  ])
}
