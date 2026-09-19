import styled from 'styled-components';
import { media, transitions } from 'helpers/style-utils';
import CSSTransitionGroup from 'react-transition-group/CSSTransitionGroup';
import Item from './Item';
const Wrapper = styled.div`
  display: block;
  margin: 1rem 0.5rem;
  ${media.mobile`margin:.5rem .25rem;`}.eir-enter {
    animation: ${transitions.stretchOut} 0.3s ease-in;
    animation-fill-mode: forwards;
  }
  .eir-leave {
    animation: ${transitions.shrinkIn} 0.15s ease-in;
    animation-fill-mode: forwards;
  }
`;
const raw = (value) => (value || '').replace(/^(imei|imeisv|imsi)-/, '');
export default ({ records, deletedId, onView, onEdit, onDelete, search }) => {
  const list = records
    .filter(
      (record) =>
        raw(record.pei).indexOf(search) !== -1 ||
        raw(record.supi).indexOf(search) !== -1
    )
    .sort((a, b) => raw(a.pei).localeCompare(raw(b.pei)))
    .map((record) => (
      <Item
        key={record._id}
        record={record}
        disabled={deletedId === record._id}
        onView={onView}
        onEdit={onEdit}
        onDelete={onDelete}
      />
    ));
  return (
    <Wrapper>
      <CSSTransitionGroup
        transitionName="eir"
        transitionEnterTimeout={300}
        transitionLeaveTimeout={150}
      >
        {list}
      </CSSTransitionGroup>
    </Wrapper>
  );
};
