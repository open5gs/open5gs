import React, { Component } from 'react';
import PropTypes from 'prop-types';

import styled from 'styled-components';
import oc from 'open-color';
import { media } from 'helpers/style-utils';

import EditIcon from 'react-icons/lib/md/edit';
import DeleteIcon from 'react-icons/lib/md/delete';

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
  overflow: hidden;
  display: flex;

  background: white;
  box-shadow: 0 1px 3px rgba(0,0,0,0.12), 0 1px 2px rgba(0,0,0,0.24);

  transition: all 0.3s cubic-bezier(.25,.8,.25,1);
  cursor: pointer;

  .actions {
    position: absolute;
    top: 0;
    right: -6rem;
    width: 6rem;
    height: 100%;
    display: flex;
    align-items: center;
    justify-content: center;

    background: ${oc.gray[1]};
    border-left: 1px solid ${oc.gray[2]};
    opacity: 0;

    transition: all .4s;
  }

  &:hover {
    box-shadow: 0 3px 6px rgba(0,0,0,0.16), 0 3px 6px rgba(0,0,0,0.23);

    .actions {
      opacity: 1;
      right: 0rem;
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
  border: 1px solid ${oc.gray[4]};
  color: ${oc.gray[4]};

  border-radius: 1rem;
  font-size: 1.15rem;

  &:hover {
    border: 1px solid ${oc.indigo[4]};
    color: ${oc.indigo[6]};
  }

  &.delete {
    &:hover {
      border: 1px solid ${oc.pink[4]};
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
          <CircleButton onClick={() => onEdit(imsi)}><EditIcon/></CircleButton>
          <CircleButton className="delete" onClick={() => onDelete(imsi)}><DeleteIcon/></CircleButton>
        </div>
      </Card>
    </Sizer>
  )
}

Item.propTypes = propTypes;

export default Item;
