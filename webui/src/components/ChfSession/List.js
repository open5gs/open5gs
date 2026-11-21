import PropTypes from 'prop-types';

import styled from 'styled-components';
import oc from 'open-color';
import { media } from 'helpers/style-utils';

import Item from './Item';

const Wrapper = styled.div`
  display: block;
  margin: 2rem;

  ${media.mobile`
    margin: 0.5rem 0.25rem;
  `}
`;

const Header = styled.div`
  display: flex;
  flex-direction: row;
  flex: 1;
  padding: 0.5rem 2.5rem;
  border-bottom: 2px solid ${oc.gray[4]};
  font-size: 0.85rem;
  font-weight: bold;
  color: ${oc.gray[6]};
  text-transform: uppercase;

  .col-ref    { width: 220px; }
  .col-supi   { width: 220px; }
  .col-dnn    { width: 120px; }
  .col-oct    { width: 120px; text-align: right; padding-right: 1rem; }
  .col-start  { width: 180px; }
`;

const propTypes = {
  sessions: PropTypes.arrayOf(PropTypes.object),
};

/* Order comes from the server (?sort=-startTime); re-sorting here could only
 * reorder within the current page, never across pages, so it is left out. */
const List = ({ sessions }) => {
  const items = sessions.map(
    s => <Item key={s.chargingDataRef} session={s} />);

  return (
    <Wrapper>
      {items.length > 0 && (
        <Header>
          <div className="col-ref">Charging Ref</div>
          <div className="col-supi">SUPI</div>
          <div className="col-dnn">DNN</div>
          <div className="col-oct">UL</div>
          <div className="col-oct">DL</div>
          <div className="col-start">Start Time</div>
        </Header>
      )}
      {items}
    </Wrapper>
  );
};

List.propTypes = propTypes;

export default List;
