import { CRUD } from 'actions/crud';
import { fromJS } from 'immutable';

const byIdInitialState = fromJS({});

export const modelInitialState = fromJS({
  byId: byIdInitialState
});

const initialState = fromJS({});

export function byIdReducer(state = byIdInitialState, action) {
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

function crud(state = initialState, action) {
  switch(action.type) {
    case CRUD.FETCH:
    case CRUD.FETCH_SUCCESS:
    case CRUD.FETCH_FAILURE:
      return state.updateIn([action.meta.model, "byId"], 
                            (s) => byIdReducer(s, action));

    default:
      return state;
  }
}

export default crud;