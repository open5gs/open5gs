import { CRUD } from 'actions/crud';
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

const modelInitialState = fromJS({
  byId: byIdInitialState,
  collections: collectionsInitialState
});

const initialState = fromJS({});

function byIdReducer(state = byIdInitialState, action) {
  switch(action.type) {
    case CRUD.FETCH_SUCCESS:
      const data = state.toJS();
      action.payload.data.forEach((document) => {
        data[document[action.meta.id]] = {
          document,
          error: null
        }
      })
      return fromJS(data);
    default:
      return state;
  }
}

function collectionReducer(state = collectionInitialState, action) {
  switch(action.type) {
    case CRUD.FETCH:
      console.log('fetch');
      return state.set('params', fromJS(action.meta.params))
                  .set('fetchedAt', 0)
                  .set('error', null);
    case CRUD.FETCH_SUCCESS:
      const data = action.payload.data;
      const ids = data.map((document) => document[action.meta.id]);
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
                            (s) => collectionsReducer(s, action));
    default:
      return state;
  }
}

export default crud;