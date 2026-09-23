import { Component } from 'react';
import styled from 'styled-components';
import oc from 'open-color';
import { media } from 'helpers/style-utils';
import EditIcon from 'react-icons/lib/md/edit';
import DeleteIcon from 'react-icons/lib/md/delete';
import { Tooltip, Spinner } from 'components';

const Sizer = styled.div`
  display: inline-block;
  width: 33.3%;
  padding: 0.5rem;
  ${(p) => p.disabled && 'opacity: 0.5; cursor: not-allowed;'};
  ${media.desktop`width: 50%;`} ${media.tablet`width: 100%;`}
`;
const Card = styled.div`
  position: relative;
  display: flex;
  background: white;
  box-shadow: 0 1px 3px rgba(0, 0, 0, 0.12), 0 1px 2px rgba(0, 0, 0, 0.24);
  transition: all 0.3s cubic-bezier(0.25, 0.8, 0.25, 1);
  cursor: pointer;
  ${(p) => p.disabled && 'pointer-events: none;'}
  .actions {
    position: absolute;
    top: 0;
    right: 0;
    width: 6rem;
    height: 100%;
    display: flex;
    align-items: center;
    justify-content: center;
    opacity: 0;
  }
  &:hover {
    box-shadow: 0 3px 6px rgba(0, 0, 0, 0.16), 0 3px 6px rgba(0, 0, 0, 0.23);
    .actions {
      ${(p) => (p.disabled ? 'opacity: 0;' : 'opacity: 1;')};
    }
  }
`;
const Pei = styled.div`
  padding-left: 1rem;
  color: ${oc.gray[8]};
  font-size: 1.25rem;
  line-height: 3rem;
`;
const CircleButton = styled.div`
  height: 2rem;
  width: 2rem;
  display: flex;
  align-items: center;
  justify-content: center;
  margin: 1px;
  background: white;
  color: ${oc.gray[6]};
  border-radius: 1rem;
  font-size: 1.5rem;
  &:hover {
    color: ${oc.indigo[6]};
  }
  &.delete:hover {
    color: ${oc.pink[6]};
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
const displayPei = (pei) => (pei || '').replace(/^(imei|imeisv)-/, '');

class Item extends Component {
  handleEdit = (e) => {
    e.stopPropagation();
    this.props.onEdit(this.props.record._id);
  };
  handleDelete = (e) => {
    e.stopPropagation();
    this.props.onDelete(this.props.record._id);
  };
  render() {
    const { disabled, record, onView } = this.props;
    return (
      <Sizer disabled={disabled}>
        <Card disabled={disabled} onClick={() => onView(record._id)}>
          <Pei>{displayPei(record.pei)}</Pei>
          <div className="actions">
            <Tooltip content="Edit" width="60px">
              <CircleButton onClick={this.handleEdit}>
                <EditIcon />
              </CircleButton>
            </Tooltip>
            <Tooltip content="Delete" width="60px">
              <CircleButton className="delete" onClick={this.handleDelete}>
                <DeleteIcon />
              </CircleButton>
            </Tooltip>
          </div>
          {disabled && (
            <SpinnerWrapper>
              <Spinner sm />
            </SpinnerWrapper>
          )}
        </Card>
      </Sizer>
    );
  }
}
export default Item;
