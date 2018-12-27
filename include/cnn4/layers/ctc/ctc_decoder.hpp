#ifndef CAFFE_CTC_DECODER_LAYER_HPP_
#define CAFFE_CTC_DECODER_LAYER_HPP_

#define CTCDecoderParameter_DEF(DEF) \
DEF##Int(blank_index, 0, 0) \
DEF##Bool(ctc_merge_repeated, true, 0) \

class CTCGreedyDecoderLayer : public Layer
{
public:
  CTCDecoderParameter_DEF(Def);
  typedef vector<int> Sequence;
  typedef vector<Sequence> Sequences;
  Sequences output_sequences_;
  int T_;
  int N_;
  int C_;

  int sequence_index_;
  int score_index_;
  int accuracy_index_;
  CTCGreedyDecoderLayer() {
    init();
  }
  virtual void init() {
    CTCDecoderParameter_DEF(Init);
    T_ = (0);
    N_ = (0);
    C_ = (0);
    sequence_index_ = (0);
    score_index_ = (-1);
    accuracy_index_ = (-1);
  }
  virtual void fromJson(cjson* param) {
    CTCDecoderParameter_DEF(Get);
  }
  virtual void toJson(cjson* param) {
    CTCDecoderParameter_DEF(Set);
  }

public:
  virtual inline const char* type() const { return "CTCDecoder"; }

  // probabilities (T x N x C),
  // sequence_indicators (T x N),[optional]
  // target_sequences (T X N) [optional]
  // if a target_sequence is provided, an additional accuracy top blob is
  // required
  virtual inline int MinBottomBlobs() const { return 1; }
  virtual inline int MaxBottomBlobs() const { return 3; }

  // sequences (terminated with negative numbers),
  // output scores [optional if 2 top blobs and bottom blobs = 2]
  // accuracy [optional, if target_sequences as bottom blob = 3]
  virtual inline int MinTopBlobs() const { return 1; }
  virtual inline int MaxTopBlobs() const { return 3; }

  const Sequences & OutputSequences() const { return output_sequences_; }

public:

  virtual void LayerSetUp(const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
#if 0
    // compute indices of output (top) blobs
    sequence_index_ = 0;  // always
    if (bottom.size() == 2) {
      // 2 input blobs (data, sequence indicators)
      if (top.size() == 1) {
        // no further output
      }
      else if (top.size() == 2) {
        score_index_ = 1;  // output scores
      }
      else {
        LOG(FATAL) << "Only two output blobs allowed: "
          << "1: sequences, 2: scores";
      }
    }
    else if (bottom.size() == 3) {
      // 3 input blobs (data, seq_ind, target_seq)
      if (top.size() == 1) {
        // no further output
      }
      else if (top.size() == 2) {
        accuracy_index_ = 1;  // output accuracy
      }
      else if (top.size() == 3) {
        score_index_ = 1;  // output scores
        accuracy_index_ = 2;  // output accuracy
      }
      else {
        LOG(FATAL) << "Need two or three output blobs: "
          << "a) 1: sequences, 2: accuracy, or "
          << "b) 1: sequences, 2: score, 3: accuracy.";
      }
    }
#else
    if (bottom.size() == 2 && top.size() == 1) { //data,label --> acc
      sequence_index_ = -1;
      accuracy_index_ = 0;
    }
    else if (bottom.size() == 1 && top.size() == 1) { //data --> decode result
      sequence_index_ = 0;
      accuracy_index_ = -1;
    }
#endif
  }

  virtual void Reshape(const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
    const Blob* probabilities = bottom[0];
    T_ = probabilities->shape(0);
    N_ = probabilities->shape(1);
    C_ = probabilities->shape(2);
    output_sequences_.clear();
    output_sequences_.resize(N_);
    if (sequence_index_ >= 0) {
      Blob* sequences = top[sequence_index_];
      sequences->Reshape(N_, T_, 1, 1);  // switch to N x T
    }
    if (score_index_ >= 0) {
      Blob* scores = top[score_index_];
      scores->Reshape(N_, 1, 1, 1);
    }
    if (accuracy_index_ >= 0) {
      Blob* accuracy = top[accuracy_index_];
      accuracy->Reshape(1, 2, 1, 1);
    }
    if (blank_index_ < 0) {
      blank_index_ = C_ - 1;
    }
  }

  virtual void Forward_(const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
#if 0
    const Blob* probabilities = bottom[0];
    const Blob* sequence_indicators = bottom[1];
    Blob* scores = (score_index_ >= 0) ? top[score_index_] : 0;
    // decode string with the requiested method (e.g. CTCGreedyDecoder)
    Decode(probabilities, sequence_indicators, &output_sequences_, scores);
    // transform output_sequences to blob
    if (sequence_index_ >= 0) {
      Blob* sequence_blob = top[sequence_index_];
      Dtype* sequence_d = sequence_blob->mdata();
      // clear all data
      caffe_set(sequence_blob->count(), static_cast<Dtype>(-1), sequence_d);
      // copy data
      for (int n = 0; n < N_; ++n) {
        Dtype* seq_n_d = sequence_d + sequence_blob->offset(0, n);
        int offset = sequence_blob->offset(1, n);
        const Sequence & output_seq = output_sequences_[n];
        CHECK_LE(output_seq.size(), T_);
        for (size_t t = 0; t < output_seq.size(); ++t) {
          *seq_n_d = output_seq[t];
          seq_n_d += offset;
        }
      }
    }
    // compute accuracy
    if (accuracy_index_ >= 0) {
      Dtype & acc = top[accuracy_index_]->mdata()[0];
      acc = 0;
      CHECK_GE(bottom.size(), 3);  // required target sequences blob
      const Blob* target_sequences_data = bottom[2];
      const Dtype* ts_data = target_sequences_data->data();
      for (int n = 0; n < N_; ++n) {
        Sequence target_sequence;
        for (int t = 0; t < T_; ++t) {
          const Dtype dtarget = ts_data[target_sequences_data->offset(t, n)];
          if (dtarget < 0) {
            // sequence has finished
            break;
          }
          // round to int, just to be sure
          const int target = static_cast<int>(0.5 + dtarget);
          target_sequence.push_back(target);
        }
        if (MAX(target_sequence.size(), output_sequences_[n].size()) == 0) {
          // 0 length
          continue;
        }
        const int ed = EditDistance(target_sequence, output_sequences_[n]);
        acc += ed * 1.0 /
          MAX(target_sequence.size(), output_sequences_[n].size());
      }
      acc = 1 - acc / N_;
      CHECK_GE(acc, 0);
      CHECK_LE(acc, 1);
    }
#else
    Blob* probabilities = bottom[0];
    // decode string with the requiested method (e.g. CTCGreedyDecoder)
    Decode(probabilities, &output_sequences_, NULL);
    // transform output_sequences to blob
    if (sequence_index_ >= 0) {
      Blob* sequence_blob = top[sequence_index_];
      Dtype* sequence_d = sequence_blob->cpu_mdata();
      // clear all data
      cpu_caffe_set(sequence_blob->count(), static_cast<Dtype>(-1), sequence_d);
      // copy data
      for (int n = 0; n < N_; ++n) {
        Dtype* seq_n_d = sequence_d + n * T_;
        const Sequence & output_seq = output_sequences_[n];
        CHECK_LE(output_seq.size(), T_);
        for (size_t t = 0; t < output_seq.size(); ++t) {
          seq_n_d[t] = output_seq[t];
        }
      }
    }
    // compute accuracy
    if (accuracy_index_ >= 0) {
      Dtype & accedit = top[0]->mdata()[0];
      Dtype & accline = top[0]->mdata()[1];
      accedit = 0;
      accline = 0;
      int total_ok = 0;
      //CHECK_GE(bottom.size(), 3);  // required target sequences blob
      Blob* target_sequences_data = bottom[1];//Batchsize x labelnum
      const Dtype* ts_data = target_sequences_data->cpu_data();
      int labelnum = target_sequences_data->channels();
      for (int n = 0; n < N_; ++n) {
        Sequence target_sequence;
        for (int t = 0; t < labelnum; ++t) {
          const Dtype dtarget = ts_data[target_sequences_data->offset(n, t)];
          if (dtarget < 0) {
            // sequence has finished
            break;
          }
          // round to int, just to be sure
          const int target = static_cast<int>(0.5 + dtarget);
          if (target != blank_index_) {
            target_sequence.push_back(target);
          }
        }
        if (MAX(target_sequence.size(), output_sequences_[n].size()) == 0) {
          // 0 length
          continue;
        }
        const int ed = EditDistance(target_sequence, output_sequences_[n]);
        accedit += ed * 1.0 /
          MAX(target_sequence.size(), output_sequences_[n].size());
        if (ed == 0) {
          total_ok++;
        }
      }
      accedit = 1 - accedit / N_;
      accline = total_ok / (float)N_;
      //LOG(INFO) << "seq all correct acc=" << total_ok/(float)N_;
      CHECK_GE(accedit, 0);
      CHECK_LE(accedit, 1);
    }
#endif
  }

  virtual void Backward_(const vector<Blob*> & top, const vector<Blob*> & bottom)
  {
    for (int i = 0; i < bottom.size(); ++i) {
      if (bottom[i]->propagate_down_) {
        //NOT_IMPLEMENTED;
      }
    }
  }

  int EditDistance(const Sequence & s1, const Sequence & s2)
  {
    const size_t len1 = s1.size();
    const size_t len2 = s2.size();
    Sequences d(len1 + 1, Sequence(len2 + 1));
    d[0][0] = 0;
    for (size_t i = 1; i <= len1; ++i) { d[i][0] = (int)i; }
    for (size_t i = 1; i <= len2; ++i) { d[0][i] = (int)i; }
    for (size_t i = 1; i <= len1; ++i) {
      for (size_t j = 1; j <= len2; ++j) {
        d[i][j] = MIN(MIN(d[i - 1][j] + 1, d[i][j - 1] + 1),
          d[i - 1][j - 1] + (s1[i - 1] == s2[j - 1] ? 0 : 1));
      }
    }
    return d[len1][len2];
  }


  virtual void Decode(Blob* probabilities, Blob* sequence_indicators, Sequences* output_sequences, Blob* scores) const
  {
    Dtype* score_data = 0;
    if (scores) {
      CHECK_EQ(scores->count(), N_);
      score_data = scores->cpu_mdata();
      cpu_caffe_set(N_, static_cast<Dtype>(0), score_data);
    }
    for (int n = 0; n < N_; ++n) {
      int prev_class_idx = -1;
      for (int t = 0; /* check at end */; ++t) {
        // get maximum probability and its index
        int max_class_idx = 0;
        const Dtype* probs = probabilities->cpu_data() + probabilities->offset(t, n);
        Dtype max_prob = probs[0];
        ++probs;
        for (int c = 1; c < C_; ++c, ++probs) {
          if (*probs > max_prob) {
            max_class_idx = c;
            max_prob = *probs;
          }
        }
        if (score_data) {
          score_data[n] += -max_prob;
        }
        if (max_class_idx != blank_index_
          && !(ctc_merge_repeated_ && max_class_idx == prev_class_idx)) {
          output_sequences->at(n).push_back(max_class_idx);
        }
        prev_class_idx = max_class_idx;
        if (t + 1 == T_ || sequence_indicators->data_at(t + 1, n, 0, 0) == 0) {
          // End of sequence
          break;
        }
      }
    }
  }

  virtual void Decode(Blob* probabilities, Sequences* output_sequences, Blob* scores) const
  {
    Dtype* score_data = 0;
    if (scores) {
      CHECK_EQ(scores->count(), N_);
      score_data = scores->cpu_mdata();
      cpu_caffe_set(N_, static_cast<Dtype>(0), score_data);
    }
    for (int n = 0; n < N_; ++n) {
      int prev_class_idx = -1;
      for (int t = 0; t < T_; ++t) {
        // get maximum probability and its index
        int max_class_idx = 0;
        const Dtype* probs = probabilities->cpu_data() + probabilities->offset(t, n);
        Dtype max_prob = probs[0];
        ++probs;
        for (int c = 1; c < C_; ++c, ++probs) {
          if (*probs > max_prob) {
            max_class_idx = c;
            max_prob = *probs;
          }
        }
        if (score_data) {
          score_data[n] += -max_prob;
        }
        if (max_class_idx != blank_index_
          && !(ctc_merge_repeated_ && max_class_idx == prev_class_idx)) {
          output_sequences->at(n).push_back(max_class_idx);
        }
        prev_class_idx = max_class_idx;
      }
    }
  }

};


INSTANTIATE_CLASS(CTCGreedyDecoder, CTCGreedyDecoderLayer);



#endif  // CAFFE_CTC_DECODER_LAYER_HPP_
