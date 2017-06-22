import React, { Component } from 'react';
import PropTypes from 'prop-types';

import styled from 'styled-components';
import oc from 'open-color';
import { media } from 'helpers/style-utils';

import StarIcon from 'react-icons/lib/md/star';
import EditIcon from 'react-icons/lib/md/edit';

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
  background: white;
  cursor: pointer;
  transition: all 0.3s cubic-bezier(.25,.8,.25,1);
  box-shadow: 0 1px 3px rgba(0,0,0,0.12), 0 1px 2px rgba(0,0,0,0.24);
  
  &:hover {
    box-shadow: 0 3px 6px rgba(0,0,0,0.16), 0 3px 6px rgba(0,0,0,0.23);
  }
`;

const Imsi = styled.div`
  padding-left: 1rem;
  font-size: 1.25rem;
  font-weight: 500;
  line-height : 3rem;
`;

const propTypes = {
  subscriber: PropTypes.shape({
    imsi: PropTypes.string
  }),
  onOpenModify: PropTypes.func,
  onOpenDelete: PropTypes.func
}

const Item = ({ subscriber, onOpenModify, onOpenDelete }) => (
  <Sizer>
    <Card onClick={onOpenModify}>
      <Imsi>{subscriber.imsi}</Imsi>
    </Card>
  </Sizer>
)

Item.propTypes = propTypes;

export default Item;
