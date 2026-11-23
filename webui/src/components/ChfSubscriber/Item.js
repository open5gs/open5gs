import { Component } from 'react';
import PropTypes from 'prop-types';

import styled from 'styled-components';
import oc from 'open-color';
import { media } from 'helpers/style-utils';

import EditIcon from 'react-icons/lib/md/edit';
import DeleteIcon from 'react-icons/lib/md/delete';

import { Tooltip, Spinner } from 'components';
import { fmtVolume, fmtPeriod, cycleUsage, scopeLabel, hasScope }
  from 'helpers/chf-quota-convert';

const Card = styled.div`
  position: relative;
  display: flex;
  padding: 0.5rem;

  cursor: pointer;

  ${p => p.disabled && 'opacity: 0.5; cursor: not-allowed; pointer-events: none;'};

  .actions {
    position: absolute;
    top: 0;
    right: 0;
    width: 8rem;
    height: 100%;
    display: flex;
    align-items: center;
    justify-content: center;
    opacity: 0;
  }

  &:hover {
    background: ${oc.gray[1]};

    .actions {
      ${p => p.disabled ? 'opacity: 0;' : 'opacity: 1;'};
    }
  }
`;

const CircleButton = styled.div`
  height: 2rem;
  width: 2rem;
  display: flex;
  align-items: center;
  justify-content: center;
  margin: 1px;

  color: ${oc.gray[6]};
  border-radius: 1rem;
  font-size: 1.5rem;

  &:hover {
    color: ${oc.indigo[6]};
  }

  &.delete {
    &:hover {
      color: ${oc.pink[6]};
    }
  }
`;

const Row = styled.div`
  display: flex;
  flex-direction: row;
  flex: 1;
  line-height: 2.5rem;
  margin: 0 2rem;

  .supi {
    font-size: 1.1rem;
    color: ${oc.gray[8]};
    width: 260px;
  }
  /* The way from a SIM's view of its pool to the pool's own page. */
  .manage-pool {
    display: inline-block;
    margin-left: 0.5rem;
    font-size: 0.8rem;
    color: ${oc.indigo[7]};
    cursor: pointer;
    &:hover { text-decoration: underline; }
  }
  /* A pool's unique id, under its name: what to search for, and what the
     TMF API and the CHF's log call it. */
  .pool-id {
    display: block;
    font-size: 0.8rem;
    font-family: monospace;
    color: ${oc.gray[7]};
  }
  /* A pool's SIMs, under its name. */
  .members {
    display: block;
    font-size: 0.78rem;
    color: ${oc.gray[6]};
    overflow: hidden;
    text-overflow: ellipsis;
    white-space: nowrap;
  }
  /* What this allowance covers: "everything" for a subscriber's
     catch-all, or the slice, DNN, PLMN and roaming state it is for. */
  .scope {
    display: block;
    font-size: 0.78rem;
    color: ${oc.gray[6]};
  }
  .quota-value {
    font-size: 1rem;
    color: ${oc.gray[6]};
    width: 130px;
  }
  .used-value {
    font-size: 1rem;
    color: ${oc.gray[6]};
    width: 190px;
  }
  /* Remaining is the number an operator scans for, so it carries the
     emphasis and goes red once the cycle allowance is gone. */
  .remaining-value {
    font-size: 1rem;
    font-weight: 600;
    width: 130px;
  }
  .bar {
    display: inline-block;
    width: 64px;
    height: 6px;
    border-radius: 3px;
    background: ${oc.gray[3]};
    margin-left: 0.5rem;
    vertical-align: middle;
    overflow: hidden;
  }
  .bar > span {
    display: block;
    height: 100%;
  }
  .period-label {
    font-size: 1rem;
    color: ${oc.gray[6]};
    width: 150px;
  }
  .renew-label {
    font-size: 1rem;
    color: ${oc.gray[6]};
    width: 100px;
  }
  .action-label {
    font-size: 1rem;
    color: ${oc.gray[6]};
    width: 160px;
  }
`;

const SpinnerWrapper = styled.div`
  position: absolute;
  top: 0;
  right: 0;
  width: 4rem;
  height: 100%;
  display: flex;
  align-items: center;
  justify-content: center;
`;


class Item extends Component {
  static propTypes = {
    chfSubscriber: PropTypes.shape({
      supi: PropTypes.string,
      members: PropTypes.arrayOf(PropTypes.string),
      bucketId: PropTypes.string,
      scope: PropTypes.object,
      priority: PropTypes.number,
      quota: PropTypes.object,
    }),
    onEdit: PropTypes.func,
    onDelete: PropTypes.func,
    /* Set on the CHF Quota view: a pool is managed on the CHF Shared Pools view,
     * so a pool's row links there instead of opening it here. */
    onManagePool: PropTypes.func,
  };

  isPool() {
    return Array.isArray(this.props.chfSubscriber.members);
  }

  handleEdit = e => {
    e.stopPropagation();
    const { chfSubscriber, onEdit, onManagePool } = this.props;
    if (this.isPool() && onManagePool) {
      onManagePool(chfSubscriber.bucketId);
      return;
    }
    onEdit(chfSubscriber.bucketId || chfSubscriber.supi);
  };

  handleDelete = e => {
    e.stopPropagation();
    const { chfSubscriber, onDelete } = this.props;
    onDelete(chfSubscriber.bucketId || chfSubscriber.supi);
  };

  render() {
    const { handleEdit, handleDelete } = this;
    const { disabled, spinner, chfSubscriber } = this.props;
    const quota = chfSubscriber.quota || {};

    /* Resolve total bytes — support both new and legacy schemas */
    const totalBytes = quota.totalQuotaBytes != null
      ? quota.totalQuotaBytes
      : ((quota.volumeUlBytes || 0) + (quota.volumeDlBytes || 0)) || null;

    /* Cycle-to-date usage from the CHF-maintained counter. */
    const usage = cycleUsage(chfSubscriber);
    /* A pool: several SIMs share this allowance. */
    const members = Array.isArray(chfSubscriber.members)
      ? chfSubscriber.members : null;
    const exhausted = usage.provisioned > 0 && usage.remaining === 0;
    const barColor = exhausted ? oc.red[6]
                   : usage.pct >= 80 ? oc.orange[5]
                   : oc.teal[6];

    return (
      <Card disabled={disabled} onClick={handleEdit}>
        <Row>
          <div className="supi">
            {members ? `Shared pool · ${members.length} SIMs` : chfSubscriber.supi}
            {members && this.props.onManagePool && (
              <span className="manage-pool" onClick={handleEdit}
                title="Open this pool on the CHF Shared Pools page">
                Manage shared pool ›
              </span>
            )}
            {members && (
              <span className="pool-id" title="The pool's id">
                {chfSubscriber.bucketId}
              </span>
            )}
            {members && (
              <span className="members" title={members.join('\n')}>
                {members.join(', ')}
              </span>
            )}
            <span className="scope" title={
                hasScope(chfSubscriber)
                  ? `This allowance covers ${scopeLabel(chfSubscriber)}; ` +
                    `priority ${chfSubscriber.priority || 0}`
                  : members ? 'Covers every session of every SIM in the pool'
                  : 'Covers every session this subscriber makes'}>
              {scopeLabel(chfSubscriber)}
            </span>
          </div>
          <div className="quota-value">
            {fmtVolume(totalBytes)}
            {usage.topup > 0 && (
              <span
                title="Top-ups bought for this billing cycle (TMF654 topupBalance)"
                style={{ color: oc.teal[7], fontSize: '0.85rem', marginLeft: '0.3rem' }}
              >
                +{fmtVolume(usage.topup)}
              </span>
            )}
          </div>
          <div className="used-value">
            {fmtVolume(usage.consumed)}
            <span className="bar">
              <span style={{
                width: `${usage.pct}%`,
                background: barColor,
              }} />
            </span>
          </div>
          <div
            className="remaining-value"
            style={{ color: exhausted ? oc.red[7] : oc.gray[8] }}
            title={usage.stale
              ? 'This billing cycle has ended. Shown is what the next session '
                + 'will be granted; the CHF opens the new cycle then.'
              : undefined}
          >
            {fmtVolume(usage.remaining)}{usage.stale ? ' *' : ''}
          </div>
          <div className="period-label">{fmtPeriod(quota.validityPeriod, quota.validitySeconds)}</div>
          <div className="renew-label">{quota.autoRenew ? 'Yes' : 'No'}</div>
          <div className="action-label">{quota.finalUnitAction || '—'}</div>
        </Row>
        <div className="actions">
          <Tooltip content="Edit" width="50px">
            <CircleButton onClick={handleEdit}><EditIcon /></CircleButton>
          </Tooltip>
          {!(members && this.props.onManagePool) && (
            <Tooltip content="Delete" width="60px">
              <CircleButton className="delete" onClick={handleDelete}><DeleteIcon /></CircleButton>
            </Tooltip>
          )}
        </div>
        {spinner && <SpinnerWrapper><Spinner sm /></SpinnerWrapper>}
      </Card>
    );
  }
}

export default Item;
