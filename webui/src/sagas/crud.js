/* @flow */
/* global Generator */

/*
import 'regenerator-runtime/runtime'
import { takeEvery, fork, put, call } from 'redux-saga/effects'

import {
  FETCH, FETCH_ONE, CREATE, UPDATE, DELETE, API_CALL, GARBAGE_COLLECT
} from './actionTypes'

// TODO: The `Effect` type is not actually defined. Because 'redux-saga' does
// not use @flow annotations, flow pretends that this import succeeds.
import type { Effect } from 'redux-saga'
import type { CrudAction } from './actionTypes'

// Generator type parameters are: Generator<+Yield,+Return,-Next>

const delay = ms => new Promise(resolve => setTimeout(resolve, ms))

function* garbageCollector() {
  yield call(delay, 10 * 60 * 1000) // initial 10 minute delay
  for (;;) {
    yield call(delay, 5 * 60 * 1000) // every 5 minutes thereafter
    yield put({ type: GARBAGE_COLLECT, meta: { now: Date.now() } })
  }
}

export const apiGeneric = (apiClient: Object) =>
function* _apiGeneric(action: CrudAction<any>): Generator<Effect, void, any> {
  const { method, path, params, data, fetchConfig } = action.payload
  const { success, failure } = action.meta
  const meta = {
    ...action.meta,
    fetchTime: Date.now()
  }

  try {
    const response = yield call(apiClient[method], path, { params, data, fetchConfig })
    yield put({ meta, type: success, payload: response })
  } catch (error) {
    yield put({ meta, type: failure, payload: error, error: true })
  }
}

const watchFetch = (apiClient) => function* _watchFetch() {
  yield* takeEvery(FETCH, apiGeneric(apiClient))
}

const watchFetchOne = (apiClient) => function* _watchFetchOne() {
  yield* takeEvery(FETCH_ONE, apiGeneric(apiClient))
}

const watchCreate = (apiClient) => function* _watchCreate() {
  yield* takeEvery(CREATE, apiGeneric(apiClient))
}

const watchUpdate = (apiClient) => function* _watchUpdate() {
  yield* takeEvery(UPDATE, apiGeneric(apiClient))
}

const watchDelete = (apiClient) => function* _watchDelete() {
  yield* takeEvery(DELETE, apiGeneric(apiClient))
}

const watchApiCall = (apiClient) => function* _watchApiCall() {
  yield* takeEvery(API_CALL, apiGeneric(apiClient))
}

export default function crudSaga(apiClient: Object) {
  return function* _crudSaga(): Generator<Effect, void, any> {
    yield [
      fork(watchFetch(apiClient)),
      fork(watchFetchOne(apiClient)),
      fork(watchCreate(apiClient)),
      fork(watchUpdate(apiClient)),
      fork(watchDelete(apiClient)),
      fork(watchApiCall(apiClient)),
      fork(garbageCollector)
    ]
  }
}

*/

import { all, takeEvery, put, call } from 'redux-saga/effects';
import { ApiClient } from 'redux-crud-store';
import { crudActions } from 'redux-crud-store';

const apiClient = new ApiClient({ basePath: '/api/db' })

function* apiGeneric(action) {
  const { method, path, params, data, fetchConfig } = action.payload
  const { success, failure } = action.meta
  const meta = {
    ...action.meta,
    fetchTime: Date.now()
  }

  try {
    const response = yield call(apiClient[method], path, { params, data, fetchConfig })
    yield put({ meta, type: success, payload: response })
  } catch (error) {
    yield put({ meta, type: failure, payload: error, error: true })
  }
}


function* watchFetch() {
  yield takeEvery(crudActions.FETCH, apiGeneric)
}

export default function* () {
  yield all([
    watchFetch()
  ])
}
