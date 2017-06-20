import React, { Component } from 'react';
import PropTypes from 'prop-types';

import styled from 'styled-components';
import oc from 'open-color';
import { media } from 'helpers/style-utils';

import StarIcon from 'react-icons/lib/md/star';
import EditIcon from 'react-icons/lib/md/edit';

const Wrapper = styled.div`
  padding: 1rem;
  position: relative;
  overflow: hidden;
  display: flex;

  background: ${oc.gray[0]};
  border: 1px solid ${oc.gray[2]};

  transition: all .25s;

  & + & {
      margin-top: 1rem;   
  }

  .actions {
    position: absolute;
    top: 0;
    right: -3rem;
    width: 3rem;
    height: 100%;
    display: flex;
    align-items: center;
    justify-content: center;
    flex-direction: column;

    background: ${oc.gray[1]};
    border-left: 1px solid ${oc.gray[2]};
    opacity: 0;

    transition: all .4s;
  }

  &:hover {
    border: 1px solid ${oc.gray[4]};
    background: white;

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
    border: 1px solid ${oc.gray[7]};
    color: ${oc.gray[9]};
  }

  &.favorite {
    ${props => props.active ? `
        border: 1px solid ${oc.yellow[6]};
        color: ${oc.yellow[6]};
    `: ''}
      
    &:active {
        border: 1px solid ${oc.yellow[6]};
        color: ${oc.yellow[6]};
    }
  }
`;

CircleButton.propTypes = {
  active: PropTypes.bool
};

const Info = styled.div`
  margin-left: 1rem;
  flex: 1;
  display: flex;
  justify-content: center;
  flex-direction: column; 
`;

/*
const Imsi = styled.div`
  font-size: 1.25rem;
  color: ${oc.gray[9]};
  font-weight: 500;
`;
*/

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

  ${media.mobile`
    width: 100%;
    padding: 0.25rem;
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
