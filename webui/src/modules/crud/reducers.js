import { CRUD } from './actions';
import { fromJS, toJS } from 'immutable';
import { isEqual } from 'lodash/lang';

const byIdInitialState = fromJS({});
const collectionInitialState = fromJS({
  params: {},
  otherInfo: {},
  ids: [],
  fetchedAt: null,
  error: null
});

const collectionsInitialState = fromJS([]);

const actionStatusInitialState = fromJS({
  create: {},
  update: {},
  delete: {}
});

const modelInitialState = fromJS({
  byId: byIdInitialState,
  collections: collectionsInitialState,
  actionStatus: actionStatusInitialState
});

const initialState = fromJS({});

function byIdReducer(state = byIdInitialState, action) {
  const idProperty = action.meta ? action.meta.idProperty : '_id';
  const id = action.meta ? action.meta.id : undefined;
  switch(action.type) {
    case CRUD.FETCH_SUCCESS:
      const data = state.toJS();
      action.payload.data.forEach((document) => {
        data[document[idProperty]] = {
          document,
          fetchedAt: action.meta.fetchedAt,
          error: null
        }
      })
      return fromJS(data);
    case CRUD.FETCH_ONE:
      return state.setIn([id, 'fetchedAt'], 0)
                  .setIn([id, 'error'], null)
    case CRUD.FETCH_ONE_SUCCESS:
      return state.setIn([id, 'fetchedAt'], action.meta.fetchedAt)
                  .setIn([id, 'error'], null)
                  .setIn([id, 'document'], fromJS(action.payload.data))
    case CRUD.FETCH_ONE_FAILURE:
      return state.setIn([id, 'fetchedAt'], action.meta.fetchedAt)
                  .setIn([id, 'error'], action.payload)
                  .setIn([id, 'document'], null)
    case CRUD.CREATE_SUCCESS:
      return state.set(action.payload.data[idProperty], fromJS({
        document: action.payload.data,
        fetchedAt: action.meta.fetchedAt,
        error: null
      }))
    case CRUD.UPDATE:
      return state.setIn([id, 'fetchedAt'], 0);
    case CRUD.UPDATE_SUCCESS:
      return state.set(id, fromJS({
        document: action.payload.data,
        fetchedAt: action.meta.fetchedAt,
        error: null
      }))
    case CRUD.DELETE_SUCCESS:
      return state.delete(id)
    default:
      return state;
  }
}

function collectionReducer(state = collectionInitialState, action) {
  switch(action.type) {
    case CRUD.FETCH:
      return state.set('params', fromJS(action.meta.params))
                  .set('fetchedAt', 0)
                  .set('error', null);
    case CRUD.FETCH_SUCCESS:
      const idProperty = action.meta ? action.meta.idProperty : '_id';
      const data = action.payload.data;
      const ids = data.map((document) => document[idProperty]);
      return state.set('params', fromJS(action.meta.params))
                  .set('ids', fromJS(ids))
                  .set('otherInfo', fromJS(action.payload).delete('data'))
                  .set('error', null)
                  .set('fetchedAt', action.meta.fetchedAt);
    case CRUD.FETCH_FAILURE:
      return state.set('params', fromJS(action.meta.params))
                  .set('error', action.payload);
    default:
      return state;
  }
}

function collectionsReducer(state = collectionsInitialState, action) {
  switch(action.type) {
    case CRUD.FETCH:
    case CRUD.FETCH_SUCCESS:
    case CRUD.FETCH_FAILURE:
      const index = state.findIndex(collection => (
        isEqual(collection.toJS().params, action.meta.params)
      ));
      if (index < 0) {
        return state.push(collectionReducer(undefined, action));
      }
      return state.update(index, s => collectionReducer(s, action));
    case CRUD.CREATE_SUCCESS:
      const idProperty = action.meta ? action.meta.idProperty : '_id';
      /* At this point, the ID is stored in the 0-index collection.
         Later you will need to find the collection to which you want to add the ID. */
      return state.update(0, item => (
        item.set('ids', item.get('ids').push(action.payload.data[idProperty]))
      ))
    case CRUD.DELETE_SUCCESS:
      const id = action.meta ? action.meta.id : undefined;
      /* Find ID from all collections and delete them if they exist. */
      return state.map((item, idx) => (
        item.set('ids', item.get('ids').filter(x => x !== id))
      ))
    default:
      return state;
  }
}

function actionStatusReducer(state = actionStatusInitialState, action) {
  const idProperty = action.meta ? action.meta.idProperty : '_id';
  const id = action.meta ? action.meta.id : undefined;
  switch(action.type) {
    case CRUD.CLEAR_ACTION_STATUS:
      return state.set(action.payload.action, fromJS({}))
    case CRUD.CREATE:
      return state.set('create', fromJS({
        pending: true,
        id: null
      }))
    case CRUD.CREATE_SUCCESS:
      return state.set('create', fromJS({
        pending: false,
        id: action.payload.data[idProperty],
        isSuccess: !action.error,
        payload: action.payload
      }))
    case CRUD.CREATE_FAILURE:
      return state.set('create', fromJS({
        pending: false,
        id: null,
        isSuccess: !action.error,
        payload: action.payload
      }))
    case CRUD.UPDATE:
      return state.set('update', fromJS({
        pending: true,
        id: id
      }))
    case CRUD.UPDATE_SUCCESS:
    case CRUD.UPDATE_FAILURE:
      return state.set('update', fromJS({
        pending: false,
        id: id,
        isSuccess: !action.error,
        payload: action.payload
      }))
    case CRUD.DELETE:
      return state.set('delete', fromJS({
        pending: true,
        id: id
      }))
    case CRUD.DELETE_SUCCESS:
    case CRUD.DELETE_FAILURE:
      return state.set('delete', fromJS({
        pending: false,
        id: id,
        isSuccess: !action.error,
        payload: action.payload
      }))
    default:
      return state;
  }
}

function crud(state = initialState, action) {
  switch(action.type) {
    case CRUD.FETCH:
    case CRUD.FETCH_SUCCESS:
    case CRUD.FETCH_FAILURE:
      return state.updateIn([action.meta.model, 'byId'],
                            (s) => byIdReducer(s, action))
                  .updateIn([action.meta.model, 'collections'],
                            (s) => collectionsReducer(s, action))
    case CRUD.FETCH_ONE:
    case CRUD.FETCH_ONE_SUCCESS:
    case CRUD.FETCH_ONE_FAILURE:
      return state.updateIn([action.meta.model, 'byId'],
                            (s) => byIdReducer(s, action))
    case CRUD.CREATE:
    case CRUD.CREATE_FAILURE:
      return state.updateIn([action.meta.model, 'actionStatus'],
                            (s) => actionStatusReducer(s, action))
    case CRUD.CREATE_SUCCESS:
      return state.updateIn([action.meta.model, 'byId'],
                            (s) => byIdReducer(s, action))
                  .updateIn([action.meta.model, 'collections'],
                            fromJS([]),
                            (s) => collectionsReducer(s, action))
                  .updateIn([action.meta.model, 'actionStatus'],
                            (s) => actionStatusReducer(s, action))
    case CRUD.UPDATE:
    case CRUD.UPDATE_SUCCESS:
      return state.updateIn([action.meta.model, 'byId'],
                            (s) => byIdReducer(s, action))
                  .updateIn([action.meta.model, 'actionStatus'],
                            (s) => actionStatusReducer(s, action))
    case CRUD.UPDATE_FAILURE:
      return state.updateIn([action.meta.model, 'actionStatus'],
                            (s) => actionStatusReducer(s, action))
    case CRUD.DELETE:
    case CRUD.DELETE_FAILURE:
      return state.updateIn([action.meta.model, 'actionStatus'],
                            (s) => actionStatusReducer(s, action))
    case CRUD.DELETE_SUCCESS:
      return state.updateIn([action.meta.model, 'byId'],
                            (s) => byIdReducer(s, action))
                  .updateIn([action.meta.model, 'collections'],
                            fromJS([]),
                            (s) => collectionsReducer(s, action))
                  .updateIn([action.meta.model, 'actionStatus'],
                            (s) => actionStatusReducer(s, action))
    case CRUD.CLEAR_ACTION_STATUS:
      return state.updateIn([action.payload.model, 'actionStatus'],
                            (s) => actionStatusReducer(s, action))
    default:
      return state;
  }
}

export default crud;