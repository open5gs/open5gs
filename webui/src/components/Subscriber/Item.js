import React, { Component } from 'react';
import PropTypes from 'prop-types';

import styled from 'styled-components';
import oc from 'open-color';
import { media } from 'helpers/style-utils';

import EditIcon from 'react-icons/lib/md/edit';
import DeleteIcon from 'react-icons/lib/md/delete';

import { Tooltip } from 'components';

const Sizer = styled.div`
  display: inline-block;
  width: 33.3%;
  padding: 0.5rem;

  ${media.desktop`
    width: 50%;
  `}

  ${media.tablet`
    width: 100%;
  `}
`;

const Card = styled.div`
  position: relative;
  display: flex;

  background: white;
  box-shadow: 0 1px 3px rgba(0,0,0,0.12), 0 1px 2px rgba(0,0,0,0.24);

  transition: all 0.3s cubic-bezier(.25,.8,.25,1);
  cursor: pointer;

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
    box-shadow: 0 3px 6px rgba(0,0,0,0.16), 0 3px 6px rgba(0,0,0,0.23);

    .actions {
      opacity: 1;
    }
  }
`;

const CircleButton = styled.div`
  height: 2rem;
  width: 2rem;
  display: flex;
  align-items: center;
  justify-content: center;
  margin: 0.25rem;

  background: white;
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
`

const Imsi = styled.div`
  padding-left: 1rem;
  color: ${oc.gray[8]};
  font-size: 1.25rem;
  line-height : 3rem;
`;

const propTypes = {
  subscriber: PropTypes.shape({
    imsi: PropTypes.string
  }),
  onShow: PropTypes.func,
  onEdit: PropTypes.func,
  onDelete: PropTypes.func
}

const Item = ({ subscriber, onShow, onEdit, onDelete }) => {
  const {
    imsi
  } = subscriber;

  return (
    <Sizer>
      <Card onClick={() => onShow(imsi)}>
        <Imsi>{subscriber.imsi}</Imsi>
        <div className="actions">
          <Tooltip content='Edit'>
            <CircleButton onClick={() => onEdit(imsi)}><EditIcon/></CircleButton>
          </Tooltip>
          <Tooltip content='Delete'>
            <CircleButton className="delete" onClick={() => onDelete(imsi)}><DeleteIcon/></CircleButton>
          </Tooltip>
        </div>
      </Card>
    </Sizer>
  )
}

Item.propTypes = propTypes;

export default Item;
