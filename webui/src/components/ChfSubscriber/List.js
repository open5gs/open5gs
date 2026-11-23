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
  border-bottom: 1px solid ${oc.gray[3]};
  font-size: 0.85rem;
  font-weight: bold;
  color: ${oc.gray[6]};
  text-transform: uppercase;

  .col-supi       { width: 260px; }
  .col-quota      { width: 130px; }
  .col-used       { width: 190px; }
  .col-remaining  { width: 130px; }
  .col-period     { width: 150px; }
  .col-renew      { width: 100px; }
  .col-action     { width: 160px; }
`;

const propTypes = {
  chfSubscribers: PropTypes.arrayOf(PropTypes.object),
  deletedSupi: PropTypes.string,
  onEdit: PropTypes.func,
  onDelete: PropTypes.func,
  onManagePool: PropTypes.func,
  /* The CHF Shared Pools view: the first column is the pool. */
  pools: PropTypes.bool,
};

/* Order comes from the server (?sort=supi); re-sorting here could only
 * reorder within the current page, never across pages, so it is left out. */
const List = ({ chfSubscribers, deletedSupi, onEdit, onDelete, onManagePool, pools }) => {
  const items = chfSubscribers
    .map(sub => (
      <Item
        key={sub.bucketId || sub.supi}
        chfSubscriber={sub}
        disabled={deletedSupi === (sub.bucketId || sub.supi)}
        spinner={deletedSupi === (sub.bucketId || sub.supi)}
        onEdit={onEdit}
        onDelete={onDelete}
        onManagePool={onManagePool}
      />
    ));

  return (
    <Wrapper>
      {items.length > 0 && (
        <Header>
          <div className="col-supi">{pools ? 'Shared pool' : 'SUPI or shared pool'}</div>
          <div className="col-quota">Provisioned</div>
          <div className="col-used">Used This Cycle</div>
          <div className="col-remaining">Remaining</div>
          <div className="col-period">Period</div>
          <div className="col-renew">Auto-Renew</div>
          <div className="col-action">Final Unit Action</div>
        </Header>
      )}
      {items}
    </Wrapper>
  );
};

List.propTypes = propTypes;

export default List;
